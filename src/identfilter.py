import sys
import re

NUMBER_REGEX = re.compile(r'([0-9])([a-z])')

def to_camel_case(text):
    # We only care about Graphene types
    if not text.startswith('graphene_') and not text.endswith('_t'):
        return text

    res = []
    for token in text[:-2].split('_'):
        uc_token = token.title()

	# We need to do this for types like graphene_point3d_t, which
	# need to be transformed into GraphenePoint3D, not GraphenePoint3d
        matches = NUMBER_REGEX.match(uc_token)
        if matches and matches.group(2):
            uc_token = ''.join([matches.group(1), matches.group(2).title])

        res.append(uc_token)

    return ''.join(res)

if __name__ == '__main__':
    in_text = sys.stdin.read()
    sys.stdout.write(to_camel_case(in_text))
