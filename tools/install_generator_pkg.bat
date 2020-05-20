@echo You need to have python and pipenv installed to run this script.

cd generator
@echo Installing package...
pipenv install -e .
cd ..
@echo Finished.
