# @command roll
# @alias r
# @permission 0
# @syntax [max]
# @description Rolls a random number between 0 and user-defined number (default 100)

from random import randint

def execute(shiro, args, user, channel) -> bool:
    max = 100

    if len(args) > 0:
        try:
            max = int(args[0])
        except ValueError:
            shiro.write('Please provide a valid number.', user.user_id, channel)
            return True

    number = randint(0, max)
    suffix = 's' if number != 1 else ''

    shiro.write_global(f'{user.presence.username} rolls {number} point{suffix}.', channel)
    return True
