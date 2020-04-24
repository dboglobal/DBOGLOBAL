CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 5;
	rq = 0;
	tid = 1416;
	title = 141602;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;252;1;";
			type = 0;
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 141607;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 45;
			}
			CDboTSClickNPC
			{
				npcidx = "1431105;";
			}
			CDboTSCheckClrQst
			{
				and = "1414;";
				flink = 1;
				flinknextqid = "1417;";
				not = 0;
			}
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 141614;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 141601;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickObject
			{
				objidx = "668;";
				widx = 1;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 141609;
				ctype = 1;
				idx = 1431105;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 141608;
				area = 141601;
				goal = 141604;
				m0fz = "4789.000000";
				m0widx = 1;
				scitem = -1;
				sort = 141605;
				stype = 1;
				m0fy = "0.000000";
				m0pat = 0;
				grade = 132203;
				m0fx = "3503.000000";
				m0ttip = 141615;
				rwd = 100;
				taid = 1;
				title = 141602;
				gtype = 1;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 141608;
			gtype = 1;
			oklnk = 2;
			area = 141601;
			goal = 141604;
			sort = 141605;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 141602;
		}
	}
}

