rem this assumes Scala is installed and in the path and that SCALA_HOME is set as an environment variable (not certain the environment variable part is required)
rem a better way to do this is to use a tool like SBT (Scala Build Tool??) etc...
cmd /C scalac -classpath "..\libs\*" ..\*.scala
pause
cmd /C scala -classpath ".;..\libs\*" SimpleApp
pause