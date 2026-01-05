#pragma once

// TODO: Now that we have all the markers, we have to figure out how to display them, and then incrementing the time. 
namespace Marker
{
	struct Marker
	{
		juce::String name = "";
		double startTimeS = 0.0;
		std::optional<double> endTimeS;
	};

	class MarkerManager
	{
	public:

		void readFile(juce::File& file)
		{
			juce::FileInputStream stream(file);
			if (!stream.openedOk())
			{
				DBG("Could not open ok! Returning...");
				return;
			}

			// Verify it's a marker.txt file...
			juce::String versionLine = stream.readNextLine();
			juce::String timeFormatLine = stream.readNextLine();
			if (!versionLine.startsWith("Marker file version:"))
			{
				DBG("First line does not start with: Marker file version:");
				DBG("Not correct file type!");
				return;
			}

			// Begin extracting each marker...
			int it = 0;
			while (!stream.isExhausted())
			{
				DBG("Reading Entry: " + std::to_string(it) + " ========");
				juce::String line = stream.readNextLine();

				// Marker entries have fields split by tabs...
				juce::StringArray fields;
				fields.addTokens(line, "\t", "");

				// Let's begin building our marker...
				Marker m;
				m.name = fields[0];
				m.startTimeS = parseTimeToSecs(fields[1]);
				if (fields[2].isNotEmpty())
				{
					m.endTimeS = parseTimeToSecs(fields[2]);
				}

				it++;

				// Let's add the marker to our main marker array.
				markers.add(m);
			}
		}

	private:
		juce::Array<Marker> markers;

		// Parses time in "HH:MM::SS.00000000", "MM:SS.00000000" or "SS.00000000" to just seconds. 
		double parseTimeToSecs(const juce::String& time)
		{
			auto parts = juce::StringArray::fromTokens(time, ":", "");

			int hours = 0;
			int mins = 0;
			double secs = 0;

			switch (parts.size())
			{
			// if SS.
			case 1:
				secs = parts[0].getDoubleValue();
				break;

			// If MM::SS.
			case 2:
				mins = parts[0].getIntValue();
				secs = parts[1].getDoubleValue();
				break;

			// If HH::MM:SS.
			case 3:
				hours = parts[0].getIntValue();
				mins = parts[1].getIntValue();
				secs = parts[2].getDoubleValue();
				break;
			}
			
			// There's up to 8 digit precision. Will the extra digits cause problems?
			double timeInSecs = hours * 3600.0 + mins * 60.0 + secs;
			DBG("Time: " + juce::String(timeInSecs));
			return timeInSecs;
		}
	};
}
