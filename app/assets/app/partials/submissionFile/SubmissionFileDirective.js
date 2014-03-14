angular.module('codepuppy').directive('submissionFile', function() {
  return {
    restrict: 'E',
    scope: {
      'file': '=file',
      'commentsReleased': '=commentsreleased', 
      'commentPermissions': '=commentpermissions'
    },
    link: function($scope) {
     $scope.$watch('file', function() {
		if($scope.file === undefined) return;
		if($scope.$$childHead.getComments === undefined) return;
		$scope.$$childHead.getComments();
     });
	},
    templateUrl: '/assets/partials/submissionFile/submissionFile.html'
  };
});
