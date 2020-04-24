CNtlTSTrigger
{
	sm = 0;
	sq = 1;
	qc = -1;
	rq = 0;
	tid = 11333;
	title = -1;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 0;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckItem
			{
				icnt = 1;
				iidx = 92003;
			}
			CDboTSClickObject
			{
				objidx = "11;";
				widx = 80000;
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
			prelnk = "0;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 254;
			prelnk = "0;";

			CDboTSActSendSvrEvt
			{
				id = 13232;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 3;
				ttype = 0;
				type = 5;
			}
			CDboTSActSendSvrEvt
			{
				id = 13260;
				tblidx = -1;
				stradius = 0;
				stype = 0;
				taid = 1;
				ttype = 255;
				type = 1;
			}
			CDboTSActObjState
			{
				mstate = 1;
				oidx = 11;
				osh_sh = 255;
				widx = 80000;
				osh_uc = 255;
				taid = 2;
			}
		}
	}
}

