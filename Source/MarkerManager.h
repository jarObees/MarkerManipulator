#pragma once

// TODO: Now that we have all the markers, we have to figure out how to display them, and then incrementing the time. 
namespace Marker
{
	struct Marker
	{
		juce::String name = "";
		double startTimeS = 0.0;
		juce::Optional<double> endTimeS;
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
				DBG("Reading Entry: " + std::to_string(it) + "========");
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

				// Let's add the marker...
				markers.add(m);
				DBG(juce::String(markers[0].startTimeS));
			}
		}

	private:
		juce::Array<Marker> markers;
		double parseTimeToSecs(const juce::String& time)
		{
			auto parts = juce::StringArray::fromTokens(time, ":", "");
			if (parts.size() != 3)
			{
				DBG("Incorrect start or end time string parsing...");
				return 0.0;
			}

			const int hours = parts[0].getIntValue();
			const int mins = parts[1].getIntValue();
			const double secs = parts[2].getDoubleValue();
			
			// There's up to 8 digit precision. Will the extra digits cause problems?
			double timeInSecs = hours * 3600.0 + mins * 60.0 + secs;
			DBG("Time: " + juce::String(timeInSecs));
			return timeInSecs;
		}
	};
}
