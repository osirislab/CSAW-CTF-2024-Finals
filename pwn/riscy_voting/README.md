
# Riscy Voting

The write-up is [here](./write-up/riscy_voting.md). See that write-up for additional notes for moderators.

## How to playtest:

```
docker compose up -d
nc localhost 7702
```

## Deployment notes:

The [`Dockerfile_original_with_builder`](./Dockerfile_original_with_builder) file is what was used to produce the binary.

The [`Dockerfile`](./Dockerfile) file is just the Docker container to deploy the challenge. The two files are separate so that deployment will be slightly faster during the competition if you have to spin up a box again during the event. 

Give people `Dockerfile_original_with_builder` along with the binary to download from `CTFd` so that they can build the challenge themselves.

Here is the SHA-256 hash of the `riscy_voting` binary to be deployed: a4939c85aec9ab94995cf565653415b201cd699692bff108fceeae61645a3b06

Giving people the actual hash may be overkill, but if anyone claims that `Dockerfile_original_with_builder` doesn't build the challenge correctly during the competition, feel free to provide the hash for troubleshooting. The `riscy_voting` binary in this directory came from copying the binary directly out of the Docker container that I spun up with `Dockerfile_original_with_builder`.