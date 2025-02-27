import os
import pandas as pd
from enum import Enum

# Define file types
class FileType(Enum):
    KOREASJ = 1

# Define data processing types
class MakeType(Enum):
    EEGEMG_ROW = 1
    EEGEMG_SUB = 2
    EEGEMG_ALL = 3

# Converter class for different transformation methods
class Converter:
    @staticmethod
    def INTEGER_ODD(x):
        return 1 if int(x) % 2 == 1 else 0

    @staticmethod
    def SIGN(x):
        return 1 if float(x) < 0 else 0

    @staticmethod
    def FRACTION(n):
        return lambda x: 1 if int(float(x) * (2 * n)) % 2 == 1 else 0

# Function to read data from files
def FileRead(fileType=FileType.KOREASJ, location="./parse/EEG_data"):
    if fileType == FileType.KOREASJ:
        all_data = {}

        # Read all Subject{n}_Ch{m}.xlsx files
        for subject in range(1, 6):
            for ch in range(1, 4):
                file_path = os.path.join(location, f"Subject{subject}_Ch{ch}.xlsx")
                if os.path.exists(file_path):
                    df = pd.read_excel(file_path, header=None)
                    all_data[(subject, ch)] = df

        return all_data
    else:
        raise ValueError("Unsupported file type.")

# Function to transform data based on MakeType
def MakeData(makeType=MakeType.EEGEMG_ROW, data=None, converter=lambda x: x):
    if data is None:
        raise ValueError("No data provided.")

    result = []

    if makeType == MakeType.EEGEMG_ROW:
        for (subject, ch), df in data.items():
            for _, row in df.iterrows():
                result.append(row.apply(converter).tolist())

    elif makeType == MakeType.EEGEMG_SUB:
        temp_result = {}
        for (subject, _), df in data.items():
            if subject not in temp_result:
                temp_result[subject] = []
            temp_result[subject].extend(df.applymap(converter).values.tolist())
        result = [sublist for sublist in temp_result.values()]

    elif makeType == MakeType.EEGEMG_ALL:
        merged_data = pd.concat(data.values(), ignore_index=True)
        result = merged_data.applymap(converter).values.tolist()

    return result

# Function to write output files
def FileWrit(location="./out", data=None, name="output", sep='\n'):
    if data is None:
        raise ValueError("No data to write.")

    os.makedirs(location, exist_ok=True)
    file_count = len(data)

    num_digits = len(str(file_count)) if file_count > 1 else 1
    for i, content in enumerate(data):
        file_name = f"{name}.txt" if file_count == 1 else f"{name}_{i+1:0{num_digits}d}.txt"
        file_path = os.path.join(location, file_name)

        with open(file_path, "w", encoding="utf-8", newline="\n") as f:
            for value in content:
                f.write(f"{value}{sep}")

    print(f"{file_count} file(s) have been saved in {location}.")

# Execution example
if __name__ == "__main__":
    data = FileRead(FileType.KOREASJ, "./parse/EEG_data")

    print("Applying INTEGER_ODD conversion:")
    transformed_data_odd = MakeData(MakeType.EEGEMG_ROW, data, converter=Converter.INTEGER_ODD)[:30]
    FileWrit("./out/row", transformed_data_odd, name="row_io",)

    print("Applying SIGN conversion:")
    transformed_data_sign = MakeData(MakeType.EEGEMG_ROW, data, converter=Converter.SIGN)[:30]
    FileWrit("./out/row", transformed_data_sign, name="row_sn")

    print("Applying FRACTION(3) conversion:")
    transformed_data_fraction = MakeData(MakeType.EEGEMG_ROW, data, converter=Converter.FRACTION(3))[:30]
    FileWrit("./out/row", transformed_data_fraction, name="row_f3")

    print("Applying FRACTION(5) conversion:")
    transformed_data_fraction = MakeData(MakeType.EEGEMG_ROW, data, converter=Converter.FRACTION(3))[:30]
    FileWrit("./out/row", transformed_data_fraction, name="row_f5")