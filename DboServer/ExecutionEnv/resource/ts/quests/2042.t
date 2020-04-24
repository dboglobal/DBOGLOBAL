CNtlTSTrigger
{
	sm = 0;
	sq = 0;
	qc = 20;
	rq = 0;
	tid = 2042;
	title = 204202;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContStart
		{
			cid = 0;
			stdiag = 204207;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckPCRace
			{
				raceflg = 1;
			}
			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 7;
			}
			CDboTSClickNPC
			{
				npcidx = "2981108;";
			}
		}
		CDboTSContEnd
		{
			cid = 254;
			prelnk = "100;";
			type = 1;
		}
		CDboTSContGCond
		{
			cid = 4;
			prelnk = "2;";
			nolnk = 255;
			rm = 0;
			yeslnk = 100;

			CDboTSRcvSvrEvt
			{
				id = 660;
			}
		}
		CDboTSContEnd
		{
			cid = 253;
			prelnk = "0;1;3;";
			type = 0;
		}
		CDboTSContGAct
		{
			cid = 3;
			elnk = 255;
			nextlnk = 253;
			prelnk = "2;";
		}
		CDboTSContReward
		{
			canclnk = 255;
			cid = 100;
			rwdzeny = 0;
			desc = 204214;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 204201;
			rwdtype = 0;
			ltime = -1;
			prelnk = "4;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "2981108;";
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 3;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 204209;
				ctype = 1;
				idx = 2981108;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 204208;
				gtype = 1;
				area = 204201;
				goal = 204204;
				grade = -1;
				rwd = 100;
				scitem = -1;
				sort = 204205;
				stype = 1;
				taid = 1;
				title = 204202;
			}
		}
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 204208;
			gtype = 1;
			oklnk = 2;
			area = 204201;
			goal = 204204;
			sort = 204205;
			prelnk = "0;";
			ds = 1;
			grade = 0;
			rwd = 100;
			title = 204202;
		}
	}
}

