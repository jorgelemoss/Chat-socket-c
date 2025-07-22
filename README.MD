# Running With Docker

#### Step 1
```
$ chmod +x docker-utils/makebuild_container.sh
```

#### Step 2
```
$ ./docker-utils/makebuild_container.sh
```

#### Step 3 (Wait for container up)
```
$ cd build && ./client
```

# Running Without Docker

## Generate a dir with files exec

<h4 style="color: red;">Just works on linux distributions</h4>

#### Step 1
```
$ ./makebuild.sh
```

#### Step 2
###### Exec server
```
$ cd build && ./server
```

#### Step 3
###### Exec client (Wait for server)
```
$ cd build && ./client
```