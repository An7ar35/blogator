## Running Blogator

//TODO

## Executing the tests

Create the docker image and run it with a stdout redirection to keep the test results in a file.

```
docker-compose create test
docker run blogator_test >> blogator-tests-output.log
```