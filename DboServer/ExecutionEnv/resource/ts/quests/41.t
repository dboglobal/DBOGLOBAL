CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 0;
	rq = 0;
	tid = 41;
	title = 4102;

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
				cont = 4108;
				gtype = 1;
				area = 4101;
				goal = 4104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 4105;
				stype = 2;
				taid = 1;
				title = 4102;
			}
			CDboTSActNPCConv
			{
				conv = 4109;
				ctype = 1;
				idx = 4072103;
				taid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 4108;
			gtype = 1;
			oklnk = 2;
			area = 4101;
			goal = 4104;
			sort = 4105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 4102;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 4114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 4101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511504;";
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
		CDboTSContStart
		{
			cid = 0;
			stdiag = 4107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckPCCls
			{
				clsflg = 3;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 8;
			}
			CDboTSClickNPC
			{
				npcidx = "4072103;";
			}
			CDboTSCheckClrQst
			{
				and = "31;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;252;";
			type = 0;
		}
	}
}

