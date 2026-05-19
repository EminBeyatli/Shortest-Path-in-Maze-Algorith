@echo off
IF "%1"=="" (
  echo Usage: push_to_github.bat https://github.com/USERNAME/REPO.git
  exit /b 1
)
set REMOTE_URL=%1

echo Initializing git repo (if not already)...
git init
git add .
git commit -m "Add maze solver project"
git branch -M main
git remote remove origin 2>nul
git remote add origin %REMOTE_URL%
echo Now push to origin/main (you may be prompted for credentials)
git push -u origin main
