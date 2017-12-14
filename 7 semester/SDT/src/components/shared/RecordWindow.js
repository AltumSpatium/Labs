import React, { Component } from 'react';
import { Link } from 'react-router-dom';
import { Button, Input, Dropdown, Modal } from 'semantic-ui-react';
import DatePicker from 'react-datepicker';
import currencies from 'constants/currencies';
import toggleModalWindow from 'utils/toggleModalWindow';
import firebaseApp from 'utils/firebase';
import moment from 'moment';

import 'react-datepicker/dist/react-datepicker.css';

const errorMessages = {
    nameLength: 'Name must be at least 4 characters long!',
    name: 'Name can only contain Latin characters and numbers!',
    newCategory: 'Category name can only contain Latin characters and numbers!',
    newCategoryLength: 'Category name must be at least 3 characters long!',
    money: 'Incorrect money value!',
    empty: 'This field must be filled!'
};

const user = localStorage['stUser'] ? JSON.parse(localStorage['stUser']) : {}; // FIX DAT
const currencyOptions = currencies.map(item => ({key: item, value: item, text: item}));

class RecordWindow extends Component {
    constructor(props) {
        super(props);

        this.state = {
            name: '',
            category: '',
            date: '',
            money: '',
            currency: '',
            account: '',

            userAccounts: [],
            userCategories: [],

            nameError: '',
            categoryError: '',
            dateError: '',
            moneyError: '',
            accountError: '',

            addCategoryOpen: false,

            newCategory: '',
            newCategoryError: ''
        };

        const recordType = this.props.recordType;

        this.accountsRef = firebaseApp.database().ref('accounts/' + user.uid);
        this.categoriesRef = firebaseApp.database().ref('categories').child(user.uid).child(recordType);

        this.onSubmitClick = this.onSubmitClick.bind(this);
        this.onCancelClick = this.onCancelClick.bind(this);
        this.toggleModalWindow = toggleModalWindow.bind(this);
        this.addNewCategory = this.addNewCategory.bind(this);
        this.onCancelNewCategory = this.onCancelNewCategory.bind(this);
        this.onChange = this.onChange.bind(this);
        this.onChangeDate = this.onChangeDate.bind(this);
        this.getAccount = this.getAccount.bind(this);
        this.validate = this.validate.bind(this);
    }

    componentWillMount() {
        this.categoriesRef.once('value', snapshot => {
            const categories = [];
            snapshot.forEach(childSnapshot => {
                const category = childSnapshot.val();
                categories.push(category);
            });
            this.setState({userCategories: categories});
        });
        this.accountsRef.once('value', snapshot => {
            const accounts = [];
            snapshot.forEach(childSnapshot => {
                const account = {
                    id: childSnapshot.key,
                    ...childSnapshot.val()
                };
                accounts.push(account);
            });
            this.setState({userAccounts: accounts});
        });
    }

    componentWillUnmount() {
        this.categoriesRef.off();
    }

    checkValidity(name, value) {
        let { nameError, categoryError, dateError, moneyError, accountError, newCategoryError } = this.state;
        switch (name) {
            case 'name':
                if (!value) nameError = errorMessages.empty;
                else if (!/^[a-zA-Z0-9 ]+$/.test(value)) nameError = errorMessages.name;
                else if (value.trim().length < 4) nameError = errorMessages.nameLength;
                else nameError = '';
                setTimeout(() => this.setState({nameError}), 500);
                break;
            case 'category':
                if (!value) categoryError = errorMessages.empty;
                else categoryError = '';
                setTimeout(() => this.setState({categoryError}), 500);
                break;
            case 'date':
                if (!value) dateError = errorMessages.empty;
                else dateError = '';
                setTimeout(() => this.setState({dateError}), 500);
                break;
            case 'money':
                if (value === '') moneyError = errorMessages.empty;
                else if (!/^[0-9]+(\.[0-9]{1,2})?$/.test(value)) moneyError = errorMessages.money;
                else moneyError = '';
                setTimeout(() => this.setState({moneyError}), 500);
                break;
            case 'account':
                if (!value) accountError = errorMessages.empty;
                else accountError = '';
                setTimeout(() => this.setState({accountError}), 500);
                break;
            case 'newCategory':
                if (!value) newCategoryError = errorMessages.empty;
                else if (!/^[a-zA-Z0-9 ]+$/.test(value)) newCategoryError = errorMessages.newCategory;
                else if (value.trim().length < 3) newCategoryError = errorMessages.newCategoryLength;
                else newCategoryError = '';
                setTimeout(() => this.setState({newCategoryError}), 500);
                break;
            default:
                break;
        }
    }

    onChange(event, data) {
        this.setState({[data.name]: data.value});
        this.checkValidity(data.name, data.value);
    }

    onChangeDate(momentObj) {
        const date = momentObj.format('YYYY/MM/DD');
        //const date = momentObj.toDate().toString();
        this.setState({date});
        this.checkValidity('date', date);
    }

    getAccount(id) {
        return this.state.userAccounts.filter(item => item.id === id)[0];
    }

    validate() {
        const { nameError, categoryError, dateError, moneyError, accountError } = this.state;
        const { name, category, date, money , account } = this.state;

        this.checkValidity('name', name);
        this.checkValidity('category', category);
        this.checkValidity('date', date);
        this.checkValidity('money', money);
        this.checkValidity('account', account);

        return name && category && date && money && account &&
            !nameError && !categoryError && !dateError && !moneyError && !accountError;
    }

    onSubmitClick(onSubmit) {
        const { name, category, date, money, currency, account } = this.state;

        if (this.validate()) {
            const accountTitle = this.getAccount(account).title;
            const record = {
                name, category, date, money, currency,
                account, displayAccount: accountTitle
            };

            onSubmit(record);
            this.setState({name: '', category: '', date: '', money: '', currency: '', account: '',
                nameError: '', categoryError: '', dateError: '', moneyError: '', accountError: '' });
        }
    }

    onCancelClick(onCancel) {
        this.setState({name: '', category: '', date: '', money: '', currency: '', account: '',
            nameError: '', categoryError: '', dateError: '', moneyError: '', accountError: '' });
        onCancel();
    };

    addNewCategory() {
        const { newCategory, newCategoryError } = this.state;
        const userCategories = this.state.userCategories.slice();

        this.checkValidity('newCategory', newCategory);
        if (newCategory && !newCategoryError) {
            userCategories.push(newCategory);
            this.setState({userCategories, newCategory: '', newCategoryError: ''});
            this.categoriesRef.push(newCategory);
            this.toggleModalWindow('addCategoryOpen');
        }
    }

    onCancelNewCategory() {
        this.setState({newCategory: '', newCategoryError: ''});
        this.toggleModalWindow('addCategoryOpen');
    }

    componentWillReceiveProps(newProps) {
        const { values: { name, category, date, money, currency='USD', account }={} } = newProps;
        this.setState({name, category, date, money, currency, account });
    }

    render() {
        const { headerText, submitText, isOpen, onSubmit, onCancel, allowFutureDate } = this.props;
        const { name, category, date, money, currency, account } = this.state;
        const categoriesOptions = this.state.userCategories.map(item => ({key: item, value: item, text: item}));
        const accountsOptions = this.state.userAccounts.map(item => ({key: item.id, value: item.id, text: item.title}));
        const dateFilter = date => allowFutureDate ? date.toDate() > moment().toDate() :
            date.toDate() <= moment().toDate();

        return (
            <Modal
                open={isOpen}
                onClose={() => this.onCancelClick(onCancel)}
                size='tiny'
                dimmer='inverted'>
                <Modal.Header>{headerText}</Modal.Header>
                <Modal.Content>
                    <div className='record-mw-fields'>
                        <Input 
                            label='Name' placeholder='Record name...' name='name'
                            fluid value={name} onChange={this.onChange} error={!!this.state.nameError} />
                        {this.state.nameError ? <p className='errorMsg'>{this.state.nameError}</p> : ''}
                        
                        <Dropdown
                            placeholder='Select category' name='category'
                            selection options={categoriesOptions} value={category}
                            onChange={this.onChange} error={!!this.state.categoryError} />
                        <Button 
                            icon='add' onClick={() => this.toggleModalWindow('addCategoryOpen')}
                            className='btn-with-offset' />
                        {this.state.categoryError ? <p className='errorMsg'>{this.state.categoryError}</p> : ''}

                        <div className={`ui input fluid ${this.state.dateError ? 'error' : ''}`}>
                            <DatePicker 
                                placeholderText='Select record date...' name='date' onChange={this.onChangeDate}
                                value={date} className='date-input' dateFormat='YYYY/MM/DD' filterDate={dateFilter} />
                        </div>
                        {this.state.dateError ? <p className='errorMsg'>{this.state.dateError}</p> : ''}

                        <Dropdown
                            placeholder='Select account' name='account'
                            selection options={accountsOptions} value={account}
                            onChange={this.onChange} error={!!this.state.accountError} />
                            <Link to='/accounts' className='ui button btn-with-offset'>Add new account</Link>
                        {this.state.accountError ? <p className='errorMsg'>{this.state.accountError}</p> : ''}

                        <Input
                            labelPosition='right' placeholder='Record money...' name='money' type='number'
                            fluid value={money} onChange={this.onChange} error={!!this.state.moneyError}
                            label={
                                <Dropdown 
                                    options={currencyOptions} value={currency}
                                    name='currency' onChange={this.onChange} />
                            } />
                        {this.state.moneyError ? <p className='errorMsg'>{this.state.moneyError}</p> : ''}
                    </div>
                    <div style={{textAlign: 'center'}}>
                        <Button positive onClick={() => this.onSubmitClick(onSubmit)}>{submitText}</Button>
                        <Button onClick={() => this.onCancelClick(onCancel)}>Cancel</Button>
                    </div>

                    <Modal
                        open={this.state.addCategoryOpen}
                        onClose={this.onCancelNewCategory}
                        size='mini'
                        dimmer='inverted'>
                        <Modal.Header>Add new category</Modal.Header>
                        <Modal.Content>
                            <div className='record-mw-fields'>
                                <Input 
                                    fluid name='newCategory' value={this.state.newCategory} placeholder='Category name...'
                                    onChange={this.onChange} error={!!this.state.newCategoryError} />
                                {this.state.newCategoryError ? <p className='errorMsg'>{this.state.newCategoryError}</p> : ''}
                            </div>
                            <div style={{textAlign: 'center'}}>
                                <Button positive onClick={this.addNewCategory}>Add</Button>
                                <Button onClick={this.onCancelNewCategory}>Cancel</Button>
                            </div>
                        </Modal.Content>
                    </Modal>
                </Modal.Content>
            </Modal>
        );
    }
}

export default RecordWindow;
