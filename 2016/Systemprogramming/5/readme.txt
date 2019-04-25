putty 내에서 프로그램 실행 방법은
$ cat input.txt | python mapper.py | python reducer.py | tee output.txt
입니다.

input.txt는 문자열을 담고있으며, output.txt는 이 문자열에 담긴 단어들의 두 쌍의 조합들과 그 조합이 input.txt 에 몇쌍이나 존재하는지에 대한 갯수를 담고 있습니다.

프로그램은 python 언어를 이용하여 제작되었으며, aws.amazon.com에서 프로그램을 실행시킬 수 있습니다.

mapper.py 에서는 한 줄씩 읽어오고, reducer.py에서는 이 한줄씩 읽어온 문자들의 조합을 기반으로 전체적인 갯수를 카운트합니다.

$ cat input.txt | python mapper.py | tee map_result.txt
$ cat map_result.txt | python reducer.py | tee output.txt
위와 같은 두 단계를 통하여 프로그램을 실행시킬 수도 있습니다.
