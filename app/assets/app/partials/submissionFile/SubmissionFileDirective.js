angular.module('codepuppy').directive('submissionFile', function() {
  return {
    restrict: 'E',
    scope: {
      'file': '=file',
      'commentsReleased': '=commentsreleased', 
      'commentPermissions': '=commentpermissions'
    },
    templateUrl: '/assets/partials/submissionFile/submissionFile.html'
  };
});
