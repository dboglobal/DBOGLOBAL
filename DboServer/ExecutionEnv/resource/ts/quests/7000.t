CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 21;
	rq = 0;
	tid = 7000;
	title = 270702;

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
				idx = 48;
				taid = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 253;
			prelnk = "1;";
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;3;";
			type = 0;
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "2;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 1;
			elnk = 3;
			nextlnk = 2;
			prelnk = "0;";

			CDboTSActMail
			{
				dtidx = 1002;
				itidx = 99155;
				stidx = 1001;
				icnt = 1;
				taid = 1;
				zenny = 0;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 270707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSParty
			{
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
		}
	}
}

