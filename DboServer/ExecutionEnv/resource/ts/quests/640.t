CNtlTSTrigger
{
	sm = 1;
	sq = 1;
	qc = 3;
	rq = 0;
	tid = 640;
	title = 64002;

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
				cont = 64008;
				gtype = 1;
				area = 64001;
				goal = 64004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 64005;
				stype = 2;
				taid = 1;
				title = 64002;
			}
			CDboTSActNPCConv
			{
				conv = 64009;
				ctype = 1;
				idx = 4152101;
				taid = 2;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 64008;
			gtype = 1;
			oklnk = 2;
			area = 64001;
			goal = 64004;
			sort = 64005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 64002;
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 64014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 64001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "2;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "3031202;";
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 64007;
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
				minlvl = 21;
			}
			CDboTSClickNPC
			{
				npcidx = "4152101;";
			}
			CDboTSCheckClrQst
			{
				and = "639;";
				flink = 1;
				flinknextqid = "672;";
				not = 0;
			}
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

