@echo off
set /p commit_message="Enter your commit message: "
git add .
git commit -m "%commit_message%"

@REM REM 현재 브랜치를 자동으로 찾고 푸시
@REM for /f "tokens=*" %%b in ('git branch --show-current') do set branch=%%b
@REM git push -u origin %branch%
