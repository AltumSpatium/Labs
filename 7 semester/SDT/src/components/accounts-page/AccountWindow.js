import React, { Component } from 'react';
import { Button, Input, Dropdown, Modal } from 'semantic-ui-react';
import currencies from 'constants/currencies';

// TODO: Improve validation, fix 'uncontrolled component warning' with title input

const errorMessages = {
    titleLength: 'Title must be at least 4 characters long!',
    title: 'Title can only contain Latin characters and numbers!',
    money: 'Incorrect money value!',
    empty: 'This field must be filled!'
};

const accountTypes = [
    {
        text: 'Cash',
        value: 'Cash'
    },
    {
        text: 'Credit card',
        value: 'Credit card'
    },
    {
        text: 'Deposit',
        value: 'Deposit'
    }
];

const currencyOptions = currencies.map(item => ({key: item, value: item, text: item}));

class AccountWindow extends Component {
    constructor(props) {
        super(props);

        this.state = {
            title: '',
            type: null,
            money: '',
            currency: '',

            titleError: '',
            typeError: '',
            moneyError: ''
        };

        this.onChange = this.onChange.bind(this);
    }

    checkValidity(name, value) {
        let { titleError, typeError, moneyError } = this.state;
        switch (name) {
            case 'title':
                if (!value) titleError = errorMessages.empty;
                else if (!/^[a-zA-Z0-9 ]+$/.test(value)) titleError = errorMessages.title;
                else if (value.trim().length < 4) titleError = errorMessages.titleLength;
                else titleError = '';
                setTimeout(() => this.setState({titleError}), 500);
                break;
            case 'type':
                if (!value) typeError = errorMessages.empty;
                else typeError = '';
                setTimeout(() => this.setState({typeError}), 500);
                break;
            case 'money':
                if (value === '') moneyError = errorMessages.empty;
                else if (!/^[0-9]+(\.[0-9]{1,2})?$/.test(value)) moneyError = errorMessages.money;
                else moneyError = '';
                setTimeout(() => this.setState({moneyError}), 500);
                break;
            default:
                break;
        }
    }

    onChange(event, data) {
        this.setState({[data.name]: data.value});
        this.checkValidity(data.name, data.value);
    }

    componentWillReceiveProps(newProps) {
        const { values: { title, type, money, currency='USD' }={} } = newProps;
        this.setState({title, type, money, currency});
    }

    render() {
        const { headerText, submitText, isOpen, onSubmit, onCancel } = this.props;
        const { title, type, money, currency } = this.state;

        const onSubmitClick = () => {
            const { titleError, typeError, moneyError } = this.state;
            const { title, type, money, currency } = this.state;

            this.checkValidity('title', title);
            this.checkValidity('type', type);
            this.checkValidity('money', money);
            if (title && type && money && !titleError && !typeError && !moneyError) {
                onSubmit({title, type, money, currency});
                this.setState({title: '', type: '', money: '', currency: '',
                    titleError: '', typeError: '', moneyError: ''});
            }
        };
        const onCancelClick = () => {
            this.setState({title: '', type: '', money: '', currency: '',
                titleError: '', typeError: '', moneyError: ''});
            onCancel();
        };

        return (
            <Modal
                open={isOpen}
                onClose={onCancelClick}
                size='tiny'
                dimmer='inverted'>
                <Modal.Header>{headerText}</Modal.Header>
                <Modal.Content>
                    <div className='account-mw-fields'>
                        <Input 
                            label='Title' placeholder='Account title...' name='title'
                            fluid value={title} onChange={this.onChange} error={!!this.state.titleError} />
                        {this.state.titleError ? <p className='errorMsg'>{this.state.titleError}</p> : ''}
                        <Dropdown 
                            placeholder='Select account type' name='type'
                            fluid selection options={accountTypes} value={type}
                            onChange={this.onChange} error={!!this.state.typeError} />
                        {this.state.typeError ? <p className='errorMsg'>{this.state.typeError}</p> : ''}
                        <Input
                            labelPosition='right' placeholder='Account money...' name='money' type='number'
                            fluid value={money} onChange={this.onChange} error={!!this.state.moneyError}
                            label={
                                <Dropdown 
                                    options={currencyOptions} value={currency}
                                    name='currency' onChange={this.onChange} />
                            } />
                        {this.state.moneyError ? <p className='errorMsg'>{this.state.moneyError}</p> : ''}
                    </div>
                    <div style={{textAlign: 'center'}}>
                        <Button positive onClick={onSubmitClick}>{submitText}</Button>
                        <Button onClick={onCancelClick}>Cancel</Button>
                    </div>
                </Modal.Content>
            </Modal>
        );
    }
}

export default AccountWindow;
