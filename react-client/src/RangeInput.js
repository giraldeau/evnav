import React, { Component } from 'react';
import { Button, Grid, Container} from 'semantic-ui-react'
import ReactSimpleRange from 'react-simple-range';

class RangeRow extends Component {
  constructor(props) {
    super(props);
    this.valueChanged = this.valueChanged.bind(this);
  }
  
  valueChanged(event) {
    if (this.props.onValueChanged) {
      const newValue = parseFloat(event.value);
      this.props.onValueChanged(this.props.index, newValue);
    }
  }
  
  render() {
    return (
      <Grid.Row>
        <Grid.Column width={4} textAlign="right">{this.props.label}</Grid.Column>
        <Grid.Column width={8}>
          <ReactSimpleRange
            style={{width: 100 + '%'}}
            type="range"
            defaultValue={this.props.value}
            min={this.props.min}
            max={this.props.max}
            step={1}
            onChange={this.valueChanged}
          />
          </Grid.Column>
        <Grid.Column width={4} textAlign="left">
          <span className="ui">{this.props.value.toFixed(this.props.decimals)} </span>
          <span className="ui">{this.props.unit}</span>
        </Grid.Column>
      </Grid.Row>
      
    );
  }
}

RangeRow.propTypes = {
  label: React.PropTypes.string,
  unit: React.PropTypes.string,
  min: React.PropTypes.number,
  max: React.PropTypes.number,
  value: React.PropTypes.number,
  step: React.PropTypes.number,
  decimals: React.PropTypes.number,
}

RangeRow.defaultProps = {
  label: "",
  unit: "",
  min: 0,
  max: 1,
  step: 0.1,
  value: 0,
  decimals: 0,
}

class ParametersPannel extends Component {

  constructor(props) {
    super(props);
    this.resetState = this.resetState.bind(this);
    this.state = {
      values: [],
    };
  }
  
  resetState() {
    var values = this.props.parameters.map((item, index) => {
      return item.defaultValue;
    });
    this.setState({values: values});
  }
  
  componentWillMount() {
    this.resetState();
  }
  
  render() {
    var content = this.state.values.map((value, index) => {
      const item = this.props.parameters[index];
      return (
        <RangeRow
          key={index}
          index={index}
          label={item.label}
          min={item.min}
          max={item.max}
          step={item.step}
          unit={item.unit}
          value={value}
          onValueChanged={(idx, val) => {
            var nextState = Object.assign({}, this.state);
            nextState.values[idx] = val;
            this.setState(nextState);
          }}
        />
      );
    });
    return (
      <Container>
        <Grid columns='equal'>
          {content}
        </Grid>
        <Container textAlign="right">
          <Button onClick={this.resetState}>Reset</Button>
          <Button color='blue'>Done</Button>
        </Container>
      </Container>
    );
  }
}

export default ParametersPannel;