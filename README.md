# IDS---Portfolio
Our project idea was based on designing an IoT system that can illustrate how the weather feels and is in a local area. These values are then compared with official meteorological data from DMI (Danmarks Meteorologiske Institut) (DMI, n.d).  

While we often as people check weather apps, they provide a number that might be hard to put into scale or visualize. Our goal with the project was to create an interactive IoT, that can give you a visual scale of what the temperature and humidity will feel like in the area around the product. The scale was decided to reflect the weather around RUC throughout the year, which lies between -5 to 30 degrees throughout the year (Vejrarkiv, DMI, 2025).  

Reliability gap  

Throughout our project we have had different challenges, where the most significant ones have been validation of data and unreliability of our DHT11 sensor. The DHT11 is a quite old and cheap sensor that often is prone to ‘noise’ in its readings, from either wind, heat interference or from hardware age (Omo-IC, 2023). 

Our challenges with the DHT11 were also the reason for the idea of including a validation of the data through the DMI API. While the DMI data provides professional data, it is recorded at the weather station – 06180 – which is in Kastrup. This results in a possible difference between readings caused either by Local Sensor Error, location differences or by the DHT11 being in either direct sun or on a cooling surface.  

The main technical focus of this project has been the combination and interaction between systems, and how to use these to communicate in a way that starts an interaction, with or from the user. The process and throughout, we learned how to integrate different systems and work with real-time data from DMI, and how to build a prototype that can be tested and improved, for future implementation.
