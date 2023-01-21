import os
from glob import glob

header = """/*****************************************************************************
* File: RnVirtualMachine.h
* Description:
* Author: Malcolm Hall
* Date: 6/19/22
* Version: 1
*
******************************************************************************/"""

for file in glob("../src/vm/**"):
    print(os.path.basename(file))
    with open(file, 'w') as f:
        contents = f.read().replace("* File:\n", f"* File: {os.path.basename(file)}")
        f.write(contents)
