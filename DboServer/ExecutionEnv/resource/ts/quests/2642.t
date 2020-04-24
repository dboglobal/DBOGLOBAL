CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 7;
	rq = 0;
	tid = 2642;
	title = 264202;

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
				conv = 264209;
				ctype = 1;
				idx = 5041150;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 264208;
				gtype = 1;
				area = 264201;
				goal = 264204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 264205;
				stype = 2;
				taid = 1;
				title = 264202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 264207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 54;
			}
			CDboTSClickNPC
			{
				npcidx = "5041150;";
			}
			CDboTSCheckClrQst
			{
				and = "2641;";
				flink = 1;
				flinknextqid = "2643;";
				not = 0;
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
			cont = 264208;
			gtype = 1;
			oklnk = 2;
			area = 264201;
			goal = 264204;
			sort = 264205;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 264202;
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
			desc = 264214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 264201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "1653117;";
			}
		}
	}
}

