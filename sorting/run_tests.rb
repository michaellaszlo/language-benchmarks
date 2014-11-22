#!/usr/bin/ruby2.0

if ARGV.length < 1 or ARGV.length > 2
	puts 'usage: run_tests.rb <n> [<result_file_name>]'
	exit
end
n = eval(ARGV[0])
if ARGV.length == 2
	result_file = open(ARGV[1], 'w')
else
	result_file = nil
end

command_templates = {
	'Python' => './sorting.py %d',
	'Ruby' => './sorting.rb %d',
	'PHP' => './sorting.php %d',
	'HHVM' => 'hhvm ./sorting.php %d',
	'C++' => './sorting %d',
	'Go' => 'go run sorting.go %d',
	'JavaScript' => 'echo %d | d8 sorting.js'}

command_templates.each do |name, template|
	command = template % n
	output = nil
	IO.popen(command) { |f| output = f.read }
	result = "\n--- %s ---\n$ %s\n%s" % [name, command, output]
	puts result
	result_file.write(result) if result_file != nil
end

puts
if result_file != nil
	result_file.write("\n")
	result_file.close()
end
