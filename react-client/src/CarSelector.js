import React, { Component } from 'react';
import { Dropdown } from 'semantic-ui-react'

const carList = [
    { text: "Nissan LEAF 24", value: "nissan-leaf-24" },
    { text: "Nissan LEAF 30", value: "nissan-leaf-30" },
    { text: "Chevrolet Spark EV", value: "chevrolet-spark-ev" },
    { text: "BMW i3 20", value: "bmw-i3-20" },
    { text: "BMW i3 30", value: "bmw-i3-30" },
    { text: "Kia SOUL", value: "kia-soul-ev" },
];

class CarSelector extends Component {
/*
  constructor(props) {
    super(props);
  }
*/
    
  render() {
    return (
      <Dropdown
        placeholder="car selection"
        fluid
        selection
        options={carList}
        onChange={(event) => {
          console.log(event.value);
        }}
      />
    );
  }
}

CarSelector.propTypes = {
}

CarSelector.defaultProps = {
}


export default CarSelector;
