angular.module('codepuppy').directive('submissionFile', function() {
  return {
    restrict: 'E',
    scope: {
      'file': '=file',
      'commentsReleased': '=commentsReleased', 
      'commentPermissions': '=commentPermissions'
    },
    templateUrl: '/assets/partials/submissionFile/submissionFile.html'
  };
});
