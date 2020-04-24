CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 6;
	rq = 0;
	tid = 1759;
	title = 175902;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 175909;
				ctype = 1;
				idx = 6111102;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 175908;
				gtype = 1;
				area = 175901;
				goal = 175904;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 175905;
				stype = 2;
				taid = 1;
				title = 175902;
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 175914;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 175901;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "6111201;";
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 175908;
			gtype = 1;
			oklnk = 2;
			area = 175901;
			goal = 175904;
			sort = 175905;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 175902;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 175907;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 50;
			}
			CDboTSCheckClrQst
			{
				and = "1726;";
				flink = 1;
				flinknextqid = "1760;";
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "6111102;";
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
	}
}

