CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 627;
	title = 62702;

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
				conv = 62709;
				ctype = 1;
				idx = 3031206;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 62708;
				gtype = 1;
				area = 62701;
				goal = 62704;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 62705;
				stype = 2;
				taid = 1;
				title = 62702;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 62707;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 2;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "3031206;";
			}
			CDboTSCheckClrQst
			{
				and = "626;";
				flink = 1;
				not = 0;
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 62708;
			gtype = 1;
			oklnk = 2;
			area = 62701;
			goal = 62704;
			sort = 62705;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 62702;
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
			desc = 62714;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 62701;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "4752105;";
			}
		}
	}
}

