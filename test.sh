###
 # @Author: Wang Zongwu
 # @Date: 2022-04-16 01:16:06
 # @LastEditTime: 2022-04-16 01:23:55
 # @LastEditors: Wang Zongwu
 # @Description: 循环测试
 # @FilePath: /Blog/share/CPP_Proj/project/pthread/memcpy_thread/test.sh
 # @Mail: wangzongwu@sjtu.edu.cn
 # Please ask for permission before quote the code.
### 
#!/bin/bash

make
for num in {1..100}
do
	./main
	if [ $? -ne 0 ]; then
		echo "error occurs"
	fi
done