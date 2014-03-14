angular.module('codepuppy').directive('submissionFileSelector', function() {
  return {
    restrict: 'E',
    scope: {
      'files': '=files',
    },
    templateUrl: '/assets/partials/submissionFileSelector/submissionFileSelector.html'
  };
});
