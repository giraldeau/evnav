import React, { Component } from 'react';
//import logo from './logo.svg';
import './App.css';
//import { Input } from 'semantic-ui-react';
import 'semantic-ui-css/semantic.css'


class RangeWidget extends Component {
  
  constructor(props) {
    super(props);
    console.log(props);
    this.step = (this.props.max - this.props.min) / this.props.resolution
    this.handleValueChange = this.handleValueChange.bind(this);
    this.state = {'value': this.props.value}
  }
  
  handleValueChange(event) {
    this.setState({value: parseFloat(event.target.value)})
  }
  
  render() {
    const value = this.state.value;
    console.log(typeof(value));
    return (
      <div className="ui segment">
        <input
            className="ui range"
            type="range"
            value={value}
            name={this.props.name}
            min={this.props.min}
            max={this.props.max}
            step={this.step}
            onChange={this.handleValueChange}
        />
        <p>{value.toFixed(1)}</p>
      </div>
    );
  }
}

RangeWidget.propTypes = {
  value: React.PropTypes.number,
}

RangeWidget.defaultProps = {
  min: 0,
  max: 1,
  resolution: 10,
  value: 0,
}

class App extends Component {
  
  constructor(props) {
    super(props);
    this.onValuechange = this.onValueChange.bind(this);
    this.state = { value: 2.0 }
  }
  
  onValueChange(id, newValue) {
    console.log(id + " " + newValue);
    this.setState({value: newValue});
  }
  
  componentDidMount() {
      console.log("go");
  }
  
  render() {
    const value = this.state.value;
    return (
      <div className="App">
        <RangeWidget 
            value={value}
            min={0}
            /* max={10} */
            resolution={100}
            name="bidon"
            valueChanged={() => {this.onValueChange()}}
        />
      </div>
    );
  }
}

export default App;
