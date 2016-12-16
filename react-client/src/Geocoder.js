import React, { Component } from 'react'
import ReactDOM from 'react-dom'
import { List, Input } from 'semantic-ui-react'
import autoBind from 'react-autobind';

class Geocoder extends Component {
  constructor() {
    super();
    autoBind(this);
    this.state = this.getDefaultState();
  }

  search(endpoint, source, accessToken, proximity, bbox, types, query, callback) {
    var searchTime = new Date();
    var uri = endpoint + '/geocoding/v5/' +
      source + '/' + encodeURIComponent(query) + '.json' +
      '?access_token=' + accessToken +
      (proximity ? '&proximity=' + proximity : '') +
      (bbox ? '&bbox=' + bbox : '') +
      (types ? '&types=' + encodeURIComponent(types) : '');
    fetch(uri, {
      headers: {
        Accept: 'application/json',
      },
    }).then(function(response) {
      return response.json();
    }).then(function(json) {
      callback(json, searchTime);
    }).catch(function(ex) {
      console.log("mapbox geocoding search error", ex);
    });
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

  onInput(event) {
    this.setState({loading: true});
    var value = event.target.value;
    if (value === '') {
      this.setState(this.getDefaultState());
    } else {
      this.search(
        this.props.endpoint,
        this.props.source,
        this.props.accessToken,
        this.props.proximity,
        this.props.bbox,
        this.props.types,
        value,
        this.onResult);
    }
  }

  moveFocus(dir) {
    if (this.state.loading)
      return;

    var focus = 0;
    if (this.state.focus !== null) {
      focus = Math.max(0, Math.min(this.state.results.length - 1, this.state.focus + dir))
    }
    console.log("focus", focus);
    this.setState({
      focus: focus,
    });
  }

  acceptFocus() {
    if (this.state.focus !== null) {
      this.props.onSelect(this.state.results[this.state.focus]);
    }
  }

  onKeyDown(event) {
    switch (event.which) {
      // up
      case 38:
        event.preventDefault();
        this.moveFocus(-1);
        break;
      // down
      case 40:
        this.moveFocus(1);
        break;
      // accept
      case 13:
        if (this.state.results.length > 0 && this.state.focus == null) {
          this.clickOption(this.state.results[0], 0);
        }
        this.acceptFocus();
        break;
      default:
        break;
    }
  }

  onResult(body, searchTime) {
    // searchTime is compared with the last search to set the state
    // to ensure that a slow xhr response does not scramble the
    // sequence of autocomplete display.
    if (body && body.features && this.state.searchTime <= searchTime) {
      this.setState({
        searchTime: searchTime,
        loading: false,
        results: body.features,
        focus: null
      });
      this.props.onSuggest(this.state.results);
    }
  }

  clickOption(place, listLocation) {
    this.props.onSelect(place);
    this.setState({focus: listLocation});
    // focus on the input after click to maintain key traversal
    ReactDOM.findDOMNode(this.refs.input).focus();
    return false;
  }

  render() {
    var resultList = null;
    if (this.state.results.length > 0) {
      const items = this.state.results.map((result, i) => {
        return (
          <List.Item key={result.id}>
            <List.Content>
              <a href="#"
                 onClick={this.clickOption.bind(this, result, i)}>
                {result.place_name}
              </a>
            </List.Content>
          </List.Item>
        );
      });

      resultList = (
        <List selection verticalAlign='middle'>
          {items}
        </List>
      );
    }

    return  (
      <div>
        <Input
          fluid
          ref="input"
          icon='location arrow'
          iconPosition='left'
          placeholder={this.props.inputPlaceholder}
          onInput={this.onInput}
          onKeyDown={this.onKeyDown}
          type='text'
        />
        {resultList}
      </div>
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