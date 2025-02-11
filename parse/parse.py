import os
import pandas as pd
import numpy as np
import matplotlib.pyplot as plt
import scipy.stats as stats

def load_eeg_data(directory='EEG_data'):
    data_list = []  # List of raw double data
    df_list = []    # DataFrame containing file names and cell positions
    
    if not os.path.exists(directory):
        print(f"Directory '{directory}' not found.")
        return [], pd.DataFrame()
    
    files = [f for f in os.listdir(directory) if f.endswith('.xlsx')]
    
    for file in files:
        file_path = os.path.join(directory, file)
        xls = pd.ExcelFile(file_path)
        
        for sheet_name in xls.sheet_names:
            df = pd.read_excel(xls, sheet_name=sheet_name, header=None)
            df = df.astype(float)  # Convert to double type
            
            # Add location information to DataFrame
            df_melted = df.melt(ignore_index=False).reset_index()
            df_melted.columns = ['Row', 'Column', 'Value']
            df_melted['File'] = file
            df_melted['Sheet'] = sheet_name
            df_list.append(df_melted)
            
            # Append raw numeric data to list
            data_list.append(df.values)
    
    combined_df = pd.concat(df_list, ignore_index=True) if df_list else pd.DataFrame()
    return data_list, combined_df

def save_csv(data_list, dataframe, list_output_file='EEG_data.csv', df_output_file='EEG_data_df.csv'):
    all_data = np.vstack(data_list)
    df_list = pd.DataFrame(all_data)
    df_list.to_csv(list_output_file, index=False, header=False)
    print(f"List data saved to {list_output_file}")
    
    dataframe.to_csv(df_output_file, index=False)
    print(f"DataFrame saved to {df_output_file}")

def plot_histogram(data_list):
    all_data = np.concatenate([arr.flatten() for arr in data_list])
    
    plt.figure(figsize=(10, 6))
    plt.hist(all_data, bins=50, edgecolor='black', alpha=0.7)
    plt.xlabel('Value')
    plt.ylabel('Frequency')
    plt.title('Histogram of EEG Data')
    plt.grid(axis='y', linestyle='--', alpha=0.7)
    plt.show()

def plot_qqplots(data_list):
    sample_data = np.random.choice(np.concatenate([arr.flatten() for arr in data_list]), size=5000, replace=False)
    
    fig, axs = plt.subplots(3, 3, figsize=(22, 18))  # Increase figure height
    plt.subplots_adjust(hspace=0.9)  # Increase vertical spacing to prevent overlap
    
    distributions = [
        ("Normal", "norm", {}),
        ("Gamma (k=2)", "gamma", {"sparams": (2,)}),
        ("Gamma (k=5)", "gamma", {"sparams": (5,)}),
        ("Student's t (df=2)", "t", {"sparams": (2,)}),
        ("Student's t (df=5)", "t", {"sparams": (5,)}),
        ("Student's t (df=10)", "t", {"sparams": (10,)}),
        ("Beta (2,5)", "beta", {"sparams": (2, 5)}),
        ("Beta (5,2)", "beta", {"sparams": (5, 2)}),
        ("Generalized t (df=5)", "t", {"sparams": (5,)})
    ]
    
    for ax, (title, dist, params) in zip(axs.flatten(), distributions):
        res = stats.probplot(sample_data, dist=dist, **params, plot=ax)
        ax.get_lines()[1].set_color('red')  # Make the reference line red
        ax.get_lines()[0].set_color('black')  # Make the data points black
        ax.get_lines()[0].set_markersize(3)  # Reduce point size
        ax.set_title(f"{title}", fontsize=14, pad=15)  # Increase padding
        ax.set_xlabel("Theoretical Quantiles", fontsize=12, labelpad=15)  # Increase label padding
        ax.set_ylabel("Ordered Values", fontsize=12, labelpad=15)  # Increase label padding
        ax.grid()
    
    plt.show()

def test_normality(data_list):
    all_data = np.concatenate([arr.flatten() for arr in data_list])
    sample_data = np.random.choice(all_data, size=5000, replace=False)

    # 평균과 분산 계산
    mean_value = np.mean(all_data)
    variance_value = np.var(all_data, ddof=1)  # ddof=1을 사용하여 표본 분산 계산

    # 정규성 검정
    shapiro_test = stats.shapiro(sample_data)
    ks_test = stats.kstest(sample_data, 'norm', args=(mean_value, np.sqrt(variance_value)))

    # 결과 출력
    print(f"Acerage: {mean_value:.6f}")
    print(f"Variance: {variance_value:.6f}")
    print(f"Shapiro-Wilk test: Statistic={shapiro_test.statistic:.6f}, p-value={shapiro_test.pvalue:.6f}")
    print(f"Kolmogorov-Smirnov test: Statistic={ks_test.statistic:.6f}, p-value={ks_test.pvalue:.6f}")


if __name__ == "__main__":
    data_list, df = load_eeg_data()
    
    if data_list:
        save_csv(data_list, df)
        plot_histogram(data_list)
        plot_qqplots(data_list)
        test_normality(data_list)
