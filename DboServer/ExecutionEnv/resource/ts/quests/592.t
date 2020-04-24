CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 2;
	rq = 0;
	tid = 592;
	title = 59202;

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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 59208;
			gtype = 1;
			oklnk = 2;
			area = 59201;
			goal = 59204;
			sort = 59205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 59202;
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 100;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 59209;
				ctype = 1;
				idx = 3171101;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 59208;
				gtype = 1;
				area = 59201;
				goal = 59204;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 59205;
				stype = 1;
				taid = 1;
				title = 59202;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 59207;
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
				minlvl = 15;
			}
			CDboTSClickNPC
			{
				npcidx = "3171101;";
			}
			CDboTSCheckClrQst
			{
				and = "504;";
				flink = 0;
				not = 0;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "1;0;252;";
			type = 0;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 59214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 59201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "7711101;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
	}
}

