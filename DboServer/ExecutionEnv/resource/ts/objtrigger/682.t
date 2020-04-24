CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 682;
	title = 68202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
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
				time = 1000;
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
				id = 51;
				tblidx = -1;
				stradius = 200;
				stype = 3;
				taid = 1;
				ttype = 0;
				type = 5;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 68207;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckAttachObj
			{
				objidx = "545;";
				widx = 1;
			}
			CDboTSCheckClrQst
			{
				and = "681;";
				flink = 0;
				not = 0;
			}
		}
	}
}

