#! /bin/bash

if [ ! -d ../V2V_Route/V2V_Route ];then
	echo "Please place V2V_Route and V2V_Route_Linux in the same directory"
	exit
fi

# First obtain the V2V_Route's git version number

cd ../V2V_Route/V2V_Route/
uq=$(git rev-list --all | head -n 1)
uq=${uq:0:7}

# Write the format version information

description="## version_"$(date +%Y)"_"$(date +%m)"_"$(date +%d)"("$uq")"

cd ../../V2V_Route_Linux

echo $description >> README.md

# Remove the old files
rm -f *.h *.cpp
rm -rf config log wt

sleep 2s

# Copy files from V2V_Route
cp -a ../V2V_Route/V2V_Route/*.cpp  ../V2V_Route/V2V_Route/*.h  .
cp -a ../V2V_Route/V2V_Route/config ../V2V_Route/V2V_Route/log ../V2V_Route/V2V_Route/wt .

sleep 2s

# Begin converting the encoding format

files=$(find . -regex ".*\.\(h\|cpp\|xml\)")

for file in $files
do
	echo "Convert encoding format: "$file
	iconv -f gbk -t utf-8 ${file} > ${file}.tmp
	rm -f ${file}
	mv ${file}.tmp  ${file}
done

# Commit

# git add .

# git commit -m ${description:3}

# git push origin master:master