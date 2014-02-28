angular.module('codepuppy').directive('commentHolder', function() {
  return {
    restrict: 'E',
    scope: {
      'commentBody': '=commentBody' // Object containing comment data.
    },
    templateUrl: '/assets/partials/commentHolder/commentHolder.html'
  };
});