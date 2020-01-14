# CPscheduler


## How to build


0) install cmake:

linux/debian: `sudo apt-get install cmake`

linux/redhat: `yum install cmake`

macOS: `brew install cmake`


1) configure your CPLEX paths on `build_release.sh`

2) run build_release.sh:


```
./build_release.sh
```

(the compiled executable will be generated on `bin/Release/CPSCHEDULER`)

3) test:

```
cd data
../bin/Release/CPSCHEDULER AB4.rcpsp
```



---

Copyright 2018, Alicanto Lab. All rights reserved.