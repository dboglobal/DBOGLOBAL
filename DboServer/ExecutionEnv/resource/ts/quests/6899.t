CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = -1;
	rq = 0;
	tid = 6899;
	title = 689902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 255;
			nextlnk = 254;
			prelnk = "1;";

			CDboTSActOutMsg
			{
				idx = 111;
				taid = 1;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 689907;
			nolnk = 255;
			rm = 0;
			yeslnk = 1;

			CDboTSQuest
			{
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 20;
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				not = 0;
				or = "6882;6883;6884;";
			}
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 255;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActMail
			{
				dtidx = 2002;
				itidx = -1;
				stidx = 2000;
				icnt = 255;
				taid = 1;
				zenny = -1;
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

