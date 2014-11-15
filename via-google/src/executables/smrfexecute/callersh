

echo "Target list file:"
echo "$1"




IFS=$'\n' read -d '' -r -a commands < $1

for command in $commands
do
	`$command`
	echo $command
done
