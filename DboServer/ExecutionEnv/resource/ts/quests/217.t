CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 1;
	rq = 0;
	tid = 217;
	title = 21702;

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
				conv = 21709;
				ctype = 1;
				idx = 4211101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 21708;
				gtype = 1;
				area = 21701;
				goal = 21704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 21705;
				stype = 1;
				taid = 1;
				title = 21702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 21707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckPCCls
			{
				clsflg = 16;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 4;
			}
			CDboTSClickNPC
			{
				npcidx = "4211101;";
			}
			CDboTSCheckClrQst
			{
				flink = 0;
				not = 0;
				or = "204;";
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
			cont = 21708;
			gtype = 1;
			oklnk = 2;
			area = 21701;
			goal = 21704;
			sort = 21705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 21702;
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
			desc = 21714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 21701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4751101;";
			}
		}
	}
}

