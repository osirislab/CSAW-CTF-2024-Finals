
# Wizard Battle

The write-up is [here](./write-up/wizard_battle.md). See that write-up for additional notes for moderators.

## How to playtest:

```
docker compose up -d
nc localhost 7701
```

## Deployment notes:

The [`Dockerfile_original_with_builder`](./Dockerfile_original_with_builder) file is what was used to produce the binary.

The [`Dockerfile`](./Dockerfile) file is just the Docker container to deploy the challenge. The two files are separate so that deployment will be slightly faster during the competition if you have to spin up a box again during the event. 

Give people `Dockerfile_original_with_builder` along with the binary to download from `CTFd` so that they can build the challenge themselves. Technically the Dockerfile is not necessary to solve the challenge, but it might make some people more confident that their solution will work remotely.

Here is the SHA-256 hash of the `wizard_battle` binary to be deployed: f972fee90e3a44c2f4cd62f6c244a503c914d495fedac7fd1bb09dc03b1598c8

Giving people the actual hash may be overkill, but if anyone claims that `Dockerfile_original_with_builder` doesn't build the challenge correctly during the competition, feel free to provide the hash for troubleshooting. The `wizard_battle` binary in this directory came from copying the binary directly out of the Docker container that I spun up with `Dockerfile_original_with_builder`.
