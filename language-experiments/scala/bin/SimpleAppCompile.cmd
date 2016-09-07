rem this assumes Scala is installed and in the path and that SCALA_HOME is set as an environment variable (not certain the environment variable part is required)
rem a better way to do this is to use a tool like SBT (Scala Build Tool??) etc...
rem the jar portion also assumes that the JDK is installed and in the PATH as well
cmd /C scalac -classpath "..\libs\*" ..\*.scala
pause
cmd /C scala -classpath ".;..\libs\*" SimpleApp
pause
rem This creates a SimpleApp.jar with the executable marker for SimpleApp.main (the main part is implied by Scala/Java)
cmd /C jar cfe SimpleApp.jar SimpleApp *.class
pause
cmd /C scala SimpleApp.jar