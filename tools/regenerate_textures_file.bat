@echo You need to have generatorpackage installed for this script.

cd generator
@echo Regenerating files...
pipenv run generate_textures_enum_definition
cd ..
@echo Finished.
