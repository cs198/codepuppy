angular.module('codepuppy').directive('commentHolder', function() {
  return {
    restrict: 'E',
    scope: {
      'commentBody': '=commentbody' // Object containing comment data.
    },
    templateUrl: '/assets/partials/commentHolder/commentHolder.html'
  };
});
