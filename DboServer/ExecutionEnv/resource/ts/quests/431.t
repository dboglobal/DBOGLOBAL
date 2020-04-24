CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 431;
	title = 43102;

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
				conv = 43109;
				ctype = 1;
				idx = 4261101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 43108;
				gtype = 1;
				area = 43101;
				goal = 43104;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 43105;
				stype = 1;
				taid = 1;
				title = 43102;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 43107;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 4;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 7;
			}
			CDboTSCheckClrQst
			{
				and = "430;";
				flink = 0;
				not = 0;
			}
			CDboTSClickNPC
			{
				npcidx = "4261101;";
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
			cont = 43108;
			gtype = 1;
			oklnk = 2;
			area = 43101;
			goal = 43104;
			sort = 43105;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 43102;
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
			desc = 43114;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 43101;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4511304;";
			}
		}
	}
}

