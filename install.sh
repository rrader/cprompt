echo "========================================"
echo "Welcome to CPrompt installer!"
DIR=`pwd`
echo "Installation directory: ${DIR}\n"
echo "1) Making"
make
echo "Making complete!\n"
echo "2) Adding enviromnent variable..."
rm "/etc/profile.d/cprompt.sh"
touch "/etc/profile.d/cprompt.sh"
echo "CPROMPTINCLUDES is `pwd`/includes/"
echo "export CPROMPTINCLUDES = `pwd`/includes/" > /etc/profile.d/cprompt.sh
echo "Ok\n"
echo "3) Creating symlink to CPrompt in /usr/bin..."
rm /usr/bin/CPrompt
ln -s "`pwd`/bin/CPrompt" "/usr/bin/CPrompt"
echo "Ok\n"
echo "Installation finished"
echo "========================================"
