set output=.\output\%1%.dll
set go=.\%1%\main.go
go build -ldflags "-s -w" -buildmode=c-shared -o %output% %go%