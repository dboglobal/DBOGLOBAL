CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 12318;
	title = 1231802;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 1;
			elnk = 253;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActWorldPlayScript
			{
				sid = 12318;
				start = 1;
				taid = 1;
				uof = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "1;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 1231807;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSRcvSvrEvt
			{
				id = 23380;
			}
		}
	}
}

