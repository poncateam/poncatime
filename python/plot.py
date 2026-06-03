import json
from plotly.subplots import make_subplots
import plotly.graph_objs as go
import numpy as np
import random

stepsKey = "steps"

jsonFile = "results.json"
with open(jsonFile, "r") as file:
    data = json.load(file)


    testNames=[]
    colors=[]

    first = next(iter(data))
    for name in data[first]:
        if name != stepsKey:
            testNames.append(name)

    for sha in data:
            # Generate random R, G, B values
            r = random.randint(0, 255)
            g = random.randint(0, 255)
            b = random.randint(0, 255)
            colors.append(f"rgb({r}, {g}, {b})")

    fig = make_subplots(rows=1, cols=len(testNames), subplot_titles=testNames)

    i = 1
    for testName in testNames:
        j=0
        for sha in data:
            x = data[sha][stepsKey]
            y = data[sha][testName]["mean"]

            stdDev = np.sqrt(data[sha][testName]["var"])
            lowerBound = y - stdDev
            upperBound = y + stdDev

            fig.add_trace(go.Scatter(x=x, y=y,
                                     name=sha,
                                     legendgroup=sha,
                                     line=dict(color=colors[j]),
                                     mode='lines',
                                     showlegend=i==1
                                     ), row=1, col=i)
            j=j+1
        i = i+1
    fig.update_layout(title_text="Side By Side Subplots")
    fig.write_html("results.html")
