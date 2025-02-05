# PlayerRegister mod

A mod that allows players to create accounts with name and password.

Useful on servers with not nessesary XBOX auth, e.g. `online-mode=false`.

## Install

```shell
lip install github.com/edshPC/PlayerRegister
```

Or download manually from [releases](https://github.com/edshPC/PlayerRegister/releases).

## Usage

When not XBOX authenticated player joins server for the first time,
it will see a form to create or login account.

XBOX authenticated players do not register accounts by default and use original id's,
but can do it by `/account create`

Passwords are SHA256-hashed

### Config

```json5
{
    // language of mod (only "en" and "ru" supported now)
    "lang": "en_US",
    // sets maximum number of accounts for one player
    "max_accounts": 3,
    // [recommended] set this to true to auto-reconnect players to the following ip&port
    // instead of kicking them when they switch accounts
    "reconnect": false, 
    "reconnect_ip": "127.0.0.1",
    "reconnect_port": 19132,
    // if true, levi's getUuid will link player's accounts to support other mods data
    "fake_uuid": true,
    // if true, fake XUID will be created for new accounts (including non-authorized players)
    "fake_xuid": true
}
```

### Command

Any permission:
* `/account create`
* `/account login`
* `/account info`

Console only:
* `/resetpassword <name>` - set random password for spec account name

## Contributing

Ask questions by creating an issue.
PRs accepted.
