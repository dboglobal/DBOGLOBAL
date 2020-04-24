CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1419;
	title = 141902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActRegQInfo
			{
				cont = 141908;
				gtype = 1;
				area = 141901;
				goal = 141904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 141905;
				stype = 1;
				taid = 1;
				title = 141902;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 141914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 141901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8755301;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 141908;
			gtype = 1;
			oklnk = 2;
			area = 141901;
			goal = 141904;
			sort = 141905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 141902;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 141907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSCheckClrQst
			{
				flink = 1;
				flinknextqid = "1420;";
				not = 0;
			}
			CDboTSClickObject
			{
				objidx = "694;";
				widx = 1;
			}
		}
	}
}

