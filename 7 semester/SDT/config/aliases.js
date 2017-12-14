const paths = require('./paths');

module.exports = {
  routes: paths.appSrc + '/routes',
  actions: paths.appSrc + '/actions',
  components: paths.appSrc + '/components',
  constants: paths.appSrc + '/constants',
  styles: paths.appSrc + '/styles',
  reducers: paths.appSrc + '/reducers',
  store: paths.appSrc + '/store',
  utils: paths.appSrc + '/utils'
};
