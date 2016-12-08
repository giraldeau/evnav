import React, { Component } from 'react'
import ReactDOM from 'react-dom'
import { Input } from 'semantic-ui-react'

class Geocoder extends Component {
  constructor() {
    super();
    this.state = this.getDefaultState();
  }
  
  getDefaultState() {
    return {
      results: [],
      focus: null,
      loading: false,
      searchTime: new Date()
    };
  };

  componentDidMount() {
    if (this.props.focusOnMount)
      ReactDOM.findDOMNode(this.refs.input).focus();
  }



  render() {
    return  (
      <Input
        fluid
        ref="input"
        icon='location arrow'
        iconPosition='left'
        placeholder='Departure'
        onChange={(event) => {
          console.log(event.target.value);
        }}
      />
    );
  }
}

Geocoder.propTypes = {
  endpoint: React.PropTypes.string,
  source: React.PropTypes.string,
  inputClass: React.PropTypes.string,
  resultClass: React.PropTypes.string,
  resultsClass: React.PropTypes.string,
  inputPosition: React.PropTypes.string,
  inputPlaceholder: React.PropTypes.string,
  resultFocusClass: React.PropTypes.string,
  onSelect: React.PropTypes.func.isRequired,
  onSuggest: React.PropTypes.func,
  accessToken: React.PropTypes.string.isRequired,
  proximity: React.PropTypes.string,
  bbox: React.PropTypes.string,
  showLoader: React.PropTypes.bool,
  focusOnMount: React.PropTypes.bool,
  types: React.PropTypes.string,
}

Geocoder.defaultProps = {
  endpoint: 'https://api.tiles.mapbox.com',
  inputClass: '',
  resultClass: '',
  resultsClass: '',
  resultFocusClass: 'strong',
  inputPosition: 'top',
  inputPlaceholder: 'Search',
  showLoader: false,
  source: 'mapbox.places',
  proximity: '',
  bbox: '',
  types: '',
  onSuggest: () => {},
  focusOnMount: true
}

export default Geocoder