CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 8163;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckAttachObj
			{
				objidx = "119;";
				widx = 717000;
			}
			CDboTSCheckObjState
			{
				mstate = 0;
				oidx = 119;
				osh = 255;
				widx = 717000;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActTWaitTS
			{
				taid = 1;
				time = 2000;
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActSendSvrEvt
			{
				id = 983;
				tblidx = 34013;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 6;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
	}
}

