CNtlTSTrigger
{
	sm = 1;
	sq = 0;
	qc = 4;
	rq = 0;
	tid = 1880;
	title = 188002;

	CNtlTSGroup
	{
		gid = 0;

		CDboTSContGAct
		{
			cid = 11;
			elnk = 255;
			nextlnk = 100;
			prelnk = "10;";

			CDboTSActPCConv
			{
				taid = 1;
				tblidx = 188028;
			}
		}
		CDboTSContGAct
		{
			cid = 4;
			elnk = 252;
			nextlnk = 10;
			prelnk = "2;";

			CDboTSActWorldPlayScript
			{
				sid = 1880;
				start = 1;
				taid = 1;
				uof = 1;
			}
		}
		CDboTSContGCond
		{
			cid = 10;
			prelnk = "4;";
			nolnk = 255;
			rm = 0;
			yeslnk = 11;

			CDboTSRcvSvrEvt
			{
				id = 386;
			}
		}
		CDboTSContGAct
		{
			cid = 2;
			elnk = 252;
			nextlnk = 4;
			prelnk = "1;";

			CDboTSActNPCConv
			{
				conv = 188009;
				ctype = 1;
				idx = 3142301;
				taid = 2;
			}
			CDboTSActRegQInfo
			{
				cont = 188008;
				gtype = 3;
				area = 188001;
				goal = 188004;
				grade = 132203;
				rwd = 100;
				scitem = -1;
				sort = 188005;
				stype = 2;
				taid = 1;
				title = 188002;
			}
		}
		CDboTSContStart
		{
			cid = 0;
			stdiag = 188007;
			nolnk = 253;
			rm = 0;
			yeslnk = 1;

			CDboTSCheckLvl
			{
				maxlvl = 100;
				minlvl = 39;
			}
			CDboTSClickNPC
			{
				npcidx = "3142301;";
			}
			CDboTSCheckClrQst
			{
				and = "1062;";
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
		CDboTSContProposal
		{
			cancellnk = 253;
			cid = 1;
			cont = 188008;
			gtype = 3;
			oklnk = 2;
			area = 188001;
			goal = 188004;
			sort = 188005;
			prelnk = "0;";
			ds = 1;
			grade = 132203;
			rwd = 100;
			title = 188002;
		}
		CDboTSContGAct
		{
			cid = 252;
			elnk = 255;
			nextlnk = 253;
			prelnk = "4;2;";
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
			desc = 188014;
			nextlnk = 254;
			rwdexp = 0;
			rwdtbl = 188001;
			rwdtype = 0;
			ltime = -1;
			prelnk = "11;";
			usetbl = 1;

			CDboTSClickNPC
			{
				npcidx = "8031401;";
			}
		}
	}
}

