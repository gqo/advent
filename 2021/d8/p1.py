sums = []

sums.append(sum([ord(c) for c in "abcefg"]))
sums.append(sum([ord(c) for c in "cf"]))
sums.append(sum([ord(c) for c in "acdeg"]))
sums.append(sum([ord(c) for c in "acdfg"]))
sums.append(sum([ord(c) for c in "bcdf"]))
sums.append(sum([ord(c) for c in "abdfg"]))
sums.append(sum([ord(c) for c in "abdefg"]))
sums.append(sum([ord(c) for c in "acf"]))
sums.append(sum([ord(c) for c in "abcdefg"]))
sums.append(sum([ord(c) for c in "abcdfg"]))

class Options():
    def __init__(self):
        self.options = {}
        self.valid = ""
        self.found = False

        for letter in "abcdefg":
            self.options[letter] = True

    def __str__(self):
        valid = "NONE"
        if self.found:
            valid = self.valid
        msg = "Op(VF?: " + str(self.found) + "; V: " + valid + "; Options: ["
        for k in self.options:
            msg += k
        msg += "])"
        return msg

    def remove_invalids(self, invalids):
        for invalid in invalids:
            if invalid in self.options:
                del self.options[invalid]

        # If one valid is left, you're done
        if len(self.options) == 1:
            self.found = True
            for k in self.options:
                self.valid = k

    def set_options(self, options):
        new_options = {}
        for c in options:
            new_options[c] = True
        
        self.options = new_options

    def get_options(self):
        valids = []
        for k in self.options:
            valids.append(k)

        return valids

    def set_valid(self, valid):
        self.valid = valid
        self.found = True
        self.options = {valid : True}

class Signal():
    def __init__(self, signal_patterns, output_values):
        # Input
        self.signal_patterns = signal_patterns
        self.output_values = output_values

        # Assumptions
        self.first_pass = self.second_pass = False
        self.one = self.four = self.seven = ""
        self.six_segments = []

        # Segment options
        self.segments = {}
        for c in "abcdefg":
            self.segments[c] = Options()

        # Translation
        self.lookup = {}

    def __str__(self):
        msg = "---- Signal ----\n"
        msg += "\tSignal Patterns: " + str(self.signal_patterns) + "\n"
        msg += "\tOutput Values: " + str(self.output_values) + "\n"
        msg += "\tSegments:" + "\n"
        for k in self.segments:
            msg += "\t\t" + "Segment(L: " + k + "; " + str(self.segments[k]) + ")" + "\n"

        if self.first_pass:
            msg += "\tFound Signal Patterns: " + "\n"
            msg += "\t\tOne: " + self.one + "\n"
            msg += "\t\tFour: " + self.four + "\n"
            msg += "\t\tSeven: " + self.seven + "\n"
            msg += "\t\tSix Segment Signals: " + str(self.six_segments) + "\n"
        msg += "----- End -----\n"

        return msg

    def primary_assumptions(self):
        if not self.first_pass:
            for sp in self.signal_patterns:
                # Save six-segment signal patterns for secondary assumptions
                if len(sp) == 6:
                    self.six_segments.append(sp)
                # Rule 1: 1 has two segments
                elif len(sp) == 2:
                    self.one = sp
                # Rule 2: 4 has four segments
                elif len(sp) == 4:
                    self.four = sp
                # Rule 3: 7 has three segments
                elif len(sp) == 3:
                    self.seven = sp

            # Rule 4: 1 is made up of segments c and f
            # Rule 1 & Rule 4 -> The segments in 1's signal pattern are either c or f
            self.segments["c"].set_options(self.one)
            self.segments["f"].set_options(self.one)

            # Previous statement -> The options for c and f aren't options for any other segment
            for k in self.segments:
                if k != "c" and k != "f":
                    self.segments[k].remove_invalids(self.one)

            # Rule 5: 2, 5, and 6 do not have all of 1's segments within them
            # !(Rule 5) -> 4 has all of 1's segments & Rule 2 -> The segments within 4 that aren't in 1 are either d or b
            segments_bd = [c for c in self.four if c not in self.one]

            self.segments["b"].set_options(segments_bd)
            self.segments["d"].set_options(segments_bd)

            # Previous statement -> The options for d and b aren't options for any other segment
            for k in self.segments:
                if k != "b" and k != "d":
                    self.segments[k].remove_invalids(segments_bd)

            # !(Rule 5) -> 7 has all of 1's segments & Rule 3 -> The segment wiithin 4 that isn't in 1 is a
            segment_a = [c for c in self.seven if c not in self.one][0]

            self.segments["a"].set_valid(segment_a)

            # Previous statement -> The segment for a is not an option for any other segment
            for k in self.segments:
                if k != "a":
                    self.segments[k].remove_invalids(segment_a)

            self.first_pass = True

    def find_missing_segment(self, letter, six_segment):
        not_elem = [c for c in self.segments[letter].get_options() if c not in six_segment]
        if len(not_elem) == 1:
            found_segment = not_elem[0]
            self.segments[letter].set_valid(found_segment)

            for k in self.segments:
                if k != letter:
                    self.segments[k].remove_invalids(found_segment)

    def secondary_assumptions(self):
        if not self.first_pass:
            print("You can't make secondary assumptions without establishing primary information!")

        if not self.second_pass:
            for six_segment in self.six_segments:
                if not self.segments["c"].found:
                    # Rule 6: 6 is the only six-segment display that does not use segment c
                    # Rule 6 -> Iff there are two remaining options for segment c, the option that is not within 6 is c
                    self.find_missing_segment("c", six_segment)

                if not self.segments["e"].found:
                    # Rule 7: 9 is the only six-segment display that does not use segment e
                    # Rule 7 -> Iff there are two remaining options for segment e, the option that is not within 9 is e
                    self.find_missing_segment("e", six_segment)

                if not self.segments["d"].found:
                    # Rule 8: 0 is the only six-segment display that does not use segment d
                    # Rule 8 -> Iff there are two remaining options for segment d, the option that is not within 0 is d
                    self.find_missing_segment("d", six_segment)

            for k in self.segments:
                self.segments[k] = self.segments[k].valid
                self.lookup[self.segments[k]] = k

            self.second_pass = True

    def output(self):
        out = ""
        for v in self.output_values:
            # Translate the segments and calculate the ordinal sum
            v_sum = sum([ord(self.lookup[c]) for c in v])

            for i in range(0, len(sums)):
                if v_sum == sums[i]:
                    # The ordinal sum of the segments of 2 and 5 is the same
                    if i == 2 or i == 5:
                        if "b" in [self.lookup[c] for c in v]:
                            out += "5"
                        else:
                            out += "2"
                    else:
                        out += str(i)

                    break
        
        return int(out)

def main():
    f = open("input.txt")
    lines = f.readlines()
    f.close()
    
    acc = 0

    for line in lines:
        signals_and_output = line.split("|")

        s = Signal(signals_and_output[0].split(), signals_and_output[1].split())
        s.primary_assumptions()
        s.secondary_assumptions()

        acc += s.output()

    print(acc)

main()