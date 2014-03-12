angular.module('codepuppy').directive('commentHolder', function() {
  return {
    restrict: 'E',
    scope: {
      'commentBody': '=commentbody', // Object containing comment data.
      'commentPermissions': '=commentpermissions'
    },
    templateUrl: '/assets/partials/commentHolder/commentHolder.html'
  };
});
