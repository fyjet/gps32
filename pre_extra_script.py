Import("env")
from shutil import copyfile

# copy User_Setup.h configuration to downloaded library
projectDir=env.get('PROJECT_DIR')
print(projectDir)
copyfile(projectDir+"/include/User_Setup.h", projectDir+"/.pio/libdeps/esp32dev/TFT_eSPI_ID1559/User_Setup.h")
